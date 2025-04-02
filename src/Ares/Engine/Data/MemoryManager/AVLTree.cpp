#include <arespch.h>
#include "Engine/Data/MemoryManager/AVLTree.h"

#include <algorithm>

#include "Engine/Data/MemoryManager/AVLNodeData.h"
#include "Engine/Data/MemoryManager/MemoryBlockNew.h"

namespace Ares::Internal {

	void AVLTree::Insert(FreeBlock* block)
	{
		// Perform the insertion
		InsertImpl(block);

		// Balance the tree if needed (after insertion)
		BalanceTree();
	}

	void AVLTree::Remove(FreeBlock* block)
	{
		// Perform the removal
		AVLNode* node = block->GetAVLNode();
		if (!node) return;

		AVLNode* parent = node->ParentPtr;
		AVLNode*& rootRef = (parent != nullptr) ? (parent->LeftPtr == node ? parent->LeftPtr : parent->RightPtr) : m_Root;

		// Check if the node has a free list
		if (node->FreeListNext != nullptr)
		{
			// Find the correct FreeBlock in the free list
			AVLNode* prev = nullptr;
			AVLNode* current = node;
			while (current && current != block->GetAVLNode())
			{
				prev = current;
				current = current->FreeListNext;
			}

			if (current != nullptr)
			{
				// Remove from free list
				if (prev != nullptr)
					prev->FreeListNext = current->FreeListNext;
				// Promote next node as head
				else
					rootRef = current->FreeListNext;
			}
			return;
		}

		// Standard AVL deletion
		if (node->LeftPtr == nullptr || node->RightPtr == nullptr)
		{
			// Case 1 & 2: One child or no child
			AVLNode* child = node->LeftPtr ? node->LeftPtr : node->RightPtr;

			if (parent != nullptr)
			{
				if (parent->LeftPtr == node)
					parent->LeftPtr = child;
				else
					parent->RightPtr = child;
			}
			else
			{
				m_Root = child;
			}

			if (child != nullptr)
				child->ParentPtr = parent;
		}
		else
		{
			// Case 3: Two children, find in-order successor
			AVLNode* successor = node->RightPtr;
			while (successor->LeftPtr)
				successor = successor->LeftPtr;

			// Swap node's data with successor
			eastl::swap(node->LeftPtr, successor->LeftPtr);
			eastl::swap(node->RightPtr, successor->RightPtr);
			eastl::swap(node->ParentPtr, successor->ParentPtr);
			eastl::swap(node->Height, successor->Height);

			if (node->ParentPtr)
			{
				if (node->ParentPtr->LeftPtr == successor)
					node->ParentPtr->LeftPtr = node;
				else
					node->ParentPtr->RightPtr = node;
			}
			else
			{
				m_Root = node;
			}
		}

		// Balance the tree from the parent upwards
		while (parent)
		{
			Balance(parent);
			parent = parent->ParentPtr;
		}
	}

	FreeBlock* AVLTree::FindBestFit(const size_t& size)
	{
		// Tree is empty, no available block
		if (m_Root == nullptr)
			return nullptr;

		// Start the search for the best fit
		AVLNode* bestFit = nullptr;
		AVLNode* current = m_Root;

		while (current != nullptr)
		{
			size_t blockSize = current->GetFreeBlock()->GetSize();

			if (blockSize >= size)
			{
				// Candidate found, but continue searching for a better fit
				bestFit = current;

				// Try smaller nodes
				current = current->LeftPtr;
			}
			else
			{
				// Too small, move to larger nodes
				current = current->RightPtr;
			}
		}

		if (bestFit == nullptr)
			return nullptr;

		return bestFit->GetFreeBlock();
	}

	AVLNode* AVLTree::Balance(AVLNode* root) const
	{
		int32_t balanceFactor = GetBalanceFactor(root);

		// If the root is left-heavy, rotate right
		if (balanceFactor > 1)
		{
			if (GetBalanceFactor(root->LeftPtr) >= 0)
			{
				return RotateRight(root);
			}
			else
			{
				root->LeftPtr = RotateLeft(root->LeftPtr);
				return RotateRight(root);
			}
		}

		// If the root is right-heavy, rotate left
		if (balanceFactor < -1)
		{
			if (GetBalanceFactor(root->RightPtr) <= 0)
			{
				return RotateLeft(root);
			}
			else
			{
				root->RightPtr = RotateRight(root->RightPtr);
				return RotateLeft(root);
			}
		}

		// If the tree is balanced, return the root as is
		return root;
	}

	void AVLTree::BalanceTree()
	{
		if (m_Root != nullptr)
			m_Root = Balance(m_Root);
	}

	void AVLTree::InsertImpl(FreeBlock* block)
	{
		// If the root is null, the first node becomes the root
		if (m_Root == nullptr)
		{
			m_Root = block->GetAVLNode();
			m_Root->LeftPtr = nullptr;
			m_Root->RightPtr = nullptr;
			m_Root->ParentPtr = nullptr;
			m_Root->Height = 0;
			m_Root->FreeListNext = nullptr;
			return;
		}

		// Perform the insertion (recursive)
		m_Root = InsertRecursive(m_Root, block);
	}

	AVLNode* AVLTree::InsertRecursive(AVLNode* root, FreeBlock* block) const
	{
		if (root == nullptr)
			return block->GetAVLNode();

		// Determine where to insert based on block size
		if (block->GetSize() < root->GetFreeBlock()->GetSize())
		{
			// Go to left subtree
			AVLNode* leftChild = InsertRecursive(root->LeftPtr, block);
			root->LeftPtr = leftChild;
			leftChild->ParentPtr = root;
		}
		else if (block->GetSize() > root->GetFreeBlock()->GetSize())
		{
			// Go to right subtree
			AVLNode* rightChild = InsertRecursive(root->RightPtr, block);
			root->RightPtr = rightChild;
			rightChild->ParentPtr = root;
		}
		else
		{
			// Block with the same size already exists!
			// TODO: Free List for blocks with same size
			AVLNode* blockNode = block->GetAVLNode();
			ReplaceNode(root, blockNode);
			blockNode->FreeListNext = root;
			return blockNode;
		}

		// Update the height after insertion
		UpdateHeight(root);

		// Return the possibly updated root
		return root;
	}

	AVLNode* AVLTree::RotateLeft(AVLNode* x) const
	{
		AVLNode* y = x->RightPtr;
		AVLNode* T2 = y->LeftPtr;

		// Perform rotation
		y->LeftPtr = x;
		x->RightPtr = T2;

		// Update parents
		if (T2 != nullptr)
			T2->ParentPtr = x;

		y->ParentPtr = x->ParentPtr;
		x->ParentPtr = y;

		// Update heights
		UpdateHeight(x);
		UpdateHeight(y);

		// New root
		return y;
	}

	AVLNode* AVLTree::RotateRight(AVLNode* y) const
	{
		AVLNode* x = y->LeftPtr;
		AVLNode* T2 = x->RightPtr;

		// Perform rotation
		x->RightPtr = y;
		y->LeftPtr = T2;

		// Update parents
		if (T2 != nullptr)
			T2->ParentPtr = y;

		x->ParentPtr = y->ParentPtr;
		y->ParentPtr = x;

		// Update heights
		UpdateHeight(y);
		UpdateHeight(x);

		// New root
		return x;
	}

	void AVLTree::UpdateHeight(AVLNode* node) const
	{
		if (node != nullptr)
			node->Height = static_cast<uint32_t>(1 + std::max<int32_t>(GetHeight(node->LeftPtr), GetHeight(node->RightPtr)));
	}

	int32_t AVLTree::GetHeight(AVLNode* node) const
	{
		return (node == nullptr) ? -1 : static_cast<int32_t>(node->Height);
	}

	int32_t AVLTree::GetBalanceFactor(AVLNode* node) const
	{
		return (node == nullptr) ? 0 : GetHeight(node->LeftPtr) - GetHeight(node->RightPtr);
	}

	AVLNode* AVLTree::GetMinValueNode(AVLNode* node) const
	{
		AVLNode* current = node;
		while (current->LeftPtr != nullptr)
			current = current->LeftPtr;
		return current;
	}

	void AVLTree::ReplaceNode(AVLNode* node, AVLNode* replacement) const
	{
		AVLNode* parent = node->ParentPtr;
		*replacement = *node;

		if (parent != nullptr)
		{
			if (parent->LeftPtr == node)
				parent->LeftPtr = replacement;
			else
				parent->RightPtr = replacement;
		}

		if (replacement->LeftPtr != nullptr)
			replacement->LeftPtr->ParentPtr = replacement;

		if (replacement->RightPtr != nullptr)
			replacement->RightPtr->ParentPtr = replacement;
	}

	/*
	AVLTree::AVLTree()
		: m_Root(nullptr)
	{
	}

	AVLTree::AVLTree(const AVLTree& other)
		: m_Root(other.m_Root)
	{
	}

	AVLTree& AVLTree::operator=(const AVLTree& other)
	{
		m_Root = other.m_Root;
		return *this;
	}

	AVLTree::AVLTree(AVLTree&& other) noexcept
		: m_Root(other.m_Root)
	{
		other.m_Root = nullptr;
	}

	AVLTree& AVLTree::operator=(AVLTree&& other) noexcept
	{
		m_Root = other.m_Root;
		other.m_Root = nullptr;

		return *this;
	}

	AVLTree::~AVLTree()
	{
		m_Root = nullptr;
	}

	void AVLTree::Insert(MemoryBlockNew& block)
	{
		// If the root is null, the first node becomes the root
		if (m_Root == nullptr)
		{
			m_Root = block.GetAVLNode();
			block.SetAVLNode(nullptr, nullptr, nullptr, 0);
			return;
		}

		// Perform the insertion (recursive function)
		m_Root = InsertRecursive(m_Root, block);

		// Balance the tree if needed (after insertion)
		m_Root = Balance(m_Root);
	}

	void AVLTree::Remove(MemoryBlockNew& block)
	{
		// If the root is null, there's nothing to remove
		if (m_Root == nullptr) return;

		// Get the AVLNodeData pointer from the memory block
		AVLNodeData* nodeToRemove = block.GetAVLNode();

		// Perform the removal (recursive function)
		m_Root = RemoveRecursive(m_Root, nodeToRemove);

		// Balance the tree if needed (after removal)
		if (m_Root != nullptr)
			m_Root = Balance(m_Root);
	}

	MemoryBlockNew AVLTree::FindBestFit(const size_t& size) const
	{
		// Tree is empty, no available block
		if (m_Root == nullptr)
			return MemoryBlockNew();

		// Start the search for the best fit
		MemoryBlockNew bestFit;
		
		FindBestFitRecursive(m_Root, size, bestFit);

		return bestFit;
	}

	AVLNodeData* AVLTree::Balance(AVLNodeData* node) const
	{
		int32_t balanceFactor = GetBalanceFactor(node);

		// If the node is left-heavy, rotate right
		if (balanceFactor > 1)
		{
			if (GetBalanceFactor(node->LeftPtr) >= 0)
				return RotateRight(node);
			else
			{
				node->LeftPtr = RotateLeft(node->LeftPtr);
				return RotateRight(node);
			}
		}

		// If the node is right-heavy, rotate left
		if (balanceFactor < -1)
		{
			if (GetBalanceFactor(node->RightPtr) <= 0)
				return RotateLeft(node);
			else
			{
				node->RightPtr = RotateRight(node->RightPtr);
				return RotateLeft(node);
			}
		}

		// If the tree is balanced, return the node as is
		return node;
	}

	AVLNodeData* AVLTree::InsertRecursive(AVLNodeData* root, MemoryBlockNew& block) const
	{
		if (root == nullptr)
			return block.GetAVLNode();

		// Determine where to insert based on block size
		if (block.GetSize() < root->GetBlockSize())
		{
			// Go to left subtree
			AVLNodeData* leftChild = InsertRecursive(root->LeftPtr, block);
			root->LeftPtr = leftChild;
			leftChild->ParentPtr = root;
		}
		else if (block.GetSize() > root->GetBlockSize())
		{
			// Got to right subtree
			AVLNodeData* rightChild = InsertRecursive(root->RightPtr, block);
			root->RightPtr = rightChild;
			rightChild->ParentPtr = root;
		}
		else
		{
			// block with the same size already exists (handle duplicates as needed)
			return root;
		}

		// Update the height after insertion
		UpdateHeight(root);

		// Return the possibly updated root
		return root;
	}

	AVLNodeData* AVLTree::RemoveRecursive(AVLNodeData* root, AVLNodeData* nodeToRemove) const
	{
		// Node to delete not found
		if (root == nullptr)
			return nullptr;


		// Compare the pointer (block AVLNodeData) with the current root's pointer
		if (nodeToRemove < root)
		{
			root->LeftPtr = RemoveRecursive(root->LeftPtr, nodeToRemove);
		}
		else if (nodeToRemove > root)
		{
			root->RightPtr = RemoveRecursive(root->RightPtr, nodeToRemove);
		}
		else
		{
			// Node to remove found
			if (root->LeftPtr == nullptr || root->RightPtr == nullptr)
			{
				// Case 1: Node has 0 or 1 child
				AVLNodeData* temp = nullptr;

				if (root->LeftPtr != nullptr)
					temp = root->LeftPtr;
				else
					temp = root->RightPtr;

				if (temp == nullptr)
				{
					// Node has no children
					temp = root;
					root = nullptr;
				}
				else
				{
					// Node has one child: replace it with the child
					*root = *temp;
				}
			}
			else
			{
				// Case 2: Node has two children
				// Find the in-order successor (smallest node in the right subtree)
				AVLNodeData* temp = GetMinValueNode(root->RightPtr);

				// Copy the in-order successor's data to the current node
				*root = *temp;

				// Remove the in-order successor
				root->RightPtr = RemoveRecursive(root->RightPtr, temp);
			}
		}

		if (root == nullptr) return root;

		// Update the height after removal
		UpdateHeight(root);

		// Balance the tree
		return Balance(root);
	}

	void AVLTree::FindBestFitRecursive(AVLNodeData* root, const size_t& size, MemoryBlockNew& bestFit) const
	{
		// Base case: if the node is null, stop recursion
		if (root == nullptr)
			return;

		// Check the current node (root)
		size_t currentBlockSize = root->GetBlockSize();
		size_t bestFitSize = bestFit.IsValid() ? bestFit.GetSize() : 0;

		if (currentBlockSize >= size && (bestFitSize == 0 || currentBlockSize < bestFitSize))
			bestFit = root->GetMemoryBlock();

		// Traverse left if the left node's block might fit better
		if (size <= currentBlockSize)
			FindBestFitRecursive(root->LeftPtr, size, bestFit);

		// Traverse right if the right node's block might fit better
		if (size >= currentBlockSize)
			FindBestFitRecursive(root->RightPtr, size, bestFit);
	}

	AVLNodeData* AVLTree::RotateLeft(AVLNodeData* x) const
	{
		AVLNodeData* y = x->RightPtr;
		AVLNodeData* T2 = y->LeftPtr;

		// Perform rotation
		y->LeftPtr = x;
		x->RightPtr = T2;

		// Update parents
		if (T2 != nullptr)
			T2->ParentPtr = x;

		y->ParentPtr = x->ParentPtr;
		x->ParentPtr = y;

		// Update heights
		UpdateHeight(x);
		UpdateHeight(y);

		// New root
		return y;
	}

	AVLNodeData* AVLTree::RotateRight(AVLNodeData* y) const
	{
		AVLNodeData* x = y->LeftPtr;
		AVLNodeData* T2 = x->RightPtr;

		// Perform rotation
		x->RightPtr = y;
		y->LeftPtr = T2;

		// Update parents
		if (T2 != nullptr)
			T2->ParentPtr = y;

		x->ParentPtr = y->ParentPtr;
		y->ParentPtr = x;

		// Update heights
		UpdateHeight(y);
		UpdateHeight(x);

		// New root
		return x;
	}

	AVLNodeData* AVLTree::GetMinValueNode(AVLNodeData* node) const
	{
		AVLNodeData* current = node;
		while (current->LeftPtr != nullptr)
		{
			current = current->LeftPtr;
		}
		return current;
	}

	void AVLTree::UpdateHeight(AVLNodeData* node) const
	{
		if (node != nullptr)
		{
			node->Height = 1 + std::max<int32_t>(GetHeight(node->LeftPtr), GetHeight(node->RightPtr));
		}
	}

	int32_t AVLTree::GetHeight(AVLNodeData* node) const
	{
		return (node == nullptr) ? -1 : node->Height;
	}

	int32_t AVLTree::GetBalanceFactor(AVLNodeData* node) const
	{
		return (node == nullptr) ? 0 : GetHeight(node->LeftPtr) - GetHeight(node->RightPtr);
	}

	void AVLTree::CheckRoot() const
	{
		if (m_Root == nullptr)
		{
			AR_CORE_ASSERT(false, "AVL Tree doesn't have a root!");
			throw std::runtime_error("AVL Tree doesn't have a root!");
		}
	}
	*/

}