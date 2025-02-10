#include <arespch.h>
#include "Engine/Data/MemoryManager/AVLTree.h"

#include <algorithm>

#include "Engine/Data/MemoryManager/AVLNodeData.h"
#include "Engine/Data/MemoryManager/MemoryBlockNew.h"

namespace Ares::Internal {

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

}