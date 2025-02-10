#pragma once

namespace Ares::Internal {

	struct AVLNodeData;
	class MemoryBlockNew;

	class AVLTree
	{
	public:
		AVLTree();
		AVLTree(const AVLTree& other);
		AVLTree& operator=(const AVLTree& other);
		AVLTree(AVLTree&&) noexcept;
		AVLTree& operator=(AVLTree&&) noexcept;
		~AVLTree();

		void Insert(MemoryBlockNew& block);
		void Remove(MemoryBlockNew& block);
		MemoryBlockNew FindBestFit(const size_t& size) const;

	private:
		AVLNodeData* Balance(AVLNodeData* node) const;
		AVLNodeData* InsertRecursive(AVLNodeData* root, MemoryBlockNew& block) const;
		AVLNodeData* RemoveRecursive(AVLNodeData* root, AVLNodeData* nodeToRemove) const;
		void FindBestFitRecursive(AVLNodeData* root, const size_t& size, MemoryBlockNew& bestFit) const;
		AVLNodeData* RotateLeft(AVLNodeData* x) const;
		AVLNodeData* RotateRight(AVLNodeData* y) const;
		AVLNodeData* GetMinValueNode(AVLNodeData* node) const;
		void UpdateHeight(AVLNodeData* node) const;
		int32_t GetHeight(AVLNodeData* node) const;
		int32_t GetBalanceFactor(AVLNodeData* node) const;
		void CheckRoot() const;

	private:
		AVLNodeData* m_Root;
	};

}