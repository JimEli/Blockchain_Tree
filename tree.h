/*************************************************************************
* Title: Binary Search Tree
* File: tree.h
* Author: James Eli
* Date: 10/26/2018
*
* Basic tree data structure using smart pointers. Basic tree functionality
* needed for assignment is included:
*
*   clear()      // deletes tree.
*   empty()      // returns true if tree is empty.
*   size()       // returns tree size (number of nodes).
*   add(T)       // recursive insert new node. does NOT check if T 
*                // already exists.
*   find(T)      // recursively find first occurrence of data in tree.
*                // returns true if T is found.
*   inOrder()    // dfs inorder recursive traversal.
*   bfs()        // bfs non-recursive traversal (top down, left to right).
*
* Bonus functions compiled if BALANCE_TREE is defined:
*   remove(T)    // Remove first occurrence of data.
*   getHeight()  // returns height of tree.
*   isBalanced() // returns true if tree is balanced.
*   balance()    // attempts to balance tree.
*
* Notes:
*  (1) Compiled/tested with MS Visual Studio 2017 Community (v141), and
*      Windows SDK version 10.0.17134.0 (32 & 64-bit).
*  (2) Compiled/tested with Eclipse Oxygen.3a Release (4.7.3a), using
*      CDT 9.4.3 and MinGw32 gcc-g++ (6.3.0-1).
*
* Submitted in partial fulfillment of the requirements of PCC CIS-269.
*************************************************************************
* Change Log:
*  10/26/2018: Initial release. JME
*************************************************************************/
#ifndef _MY_TREE_H_
#define _MY_TREE_H_

#include <iostream>  // cout.
#include <memory>    // shared pointers.
#include <algorithm> // max.
// Using my data structures.
#include "queue.h"   // bfs traversal.
#include "vector.h"  // vector for building balanced tree.

using namespace myQueue;
using namespace myVector;

namespace myTree {

	template <class T>
	class Tree
	{
	private:
		struct Node
		{
		private:
			T data;                      // Node data element.
			std::shared_ptr<Node> left;  // Left child.
			std::shared_ptr<Node> right; // Right child.

			// Return true if node is leaf.
			bool isLeaf() const { return !left && !right; }
		
		public:
			explicit Node(T d) : data(d), left(nullptr), right(nullptr) { }
			~Node() = default;
		
			friend class Tree;
		};

		// Tree root node.
		std::shared_ptr<Node> root;
	
	public:
		Tree() : root(nullptr) { }

		~Tree() = default;
	
		// Clear all nodes from tree.
		void clear() { clear(root); }
		
		// Return true if tree is empty.
		bool empty() const { return (root == nullptr); }
		
		// Return node count.
		std::size_t size() { return size(root); }
		
		// Insert item into tree.
		void add(T data) { add(root, data); }
		
		// recursive search.
		bool find(T data) const { return find(root, data); }
		
		// Dfs in-order traversal (recursive).
		void inOrder() const { inOrder(root); }
		
		// Bfs traversal (top down, left to right).
		void bfs() const { bfs(root); }

#ifdef BALANCE_TREE
		// Remove first occurrence of data.
		bool remove(T data) { return remove(root, data); }

		// Get height of node. Used by isBalanced function.
		int getHeight() { return getHeight(root); }
		
		// Recursive check of tree balance. Returns true if tree is balanced.
		bool isBalanced() { return isBalanced(root); }
		
		// Attempt to balance tree.
		void balance() { balanceTree(root); }
#endif // End BALANCE_TREE.

	private:
		// Delete all nodes of tree.
		void clear(std::shared_ptr<Node> &node)
		{
			if (node->left)
				clear(node->left);
			if (node->right)
				clear(node->right);
			node.reset(); 
		}

		// Number of nodes in tree.
		std::size_t size(std::shared_ptr<Node> &node)
		{
			if (!node)
				return 0;
			else
				return(size(node->left) + 1 + size(node->right));
		}

		// Add new node to tree.
		void add(std::shared_ptr<Node> &node, T &data)
		{
			if (!node)
				node = std::make_shared<Node>(data);
			else
				data < node->data ? add(node->left, data) : add(node->right, data);
		}

		// Recursively find first occurrence of data in tree.
		bool find(std::shared_ptr<Node> node, T &data) const
		{
			if (!node)
				return false;
			else
			{
				if (node->data == data)
					return true;

				return find(node->left, data) || find(node->right, data);
			}
		}
		
		// Dfs in-order traversal.
		void inOrder(const std::shared_ptr<Node> node) const
		{
			if (node->left)
				inOrder(node->left);
			std::cout << node->data;
			if (node->right)
				inOrder(node->right);
		}

		// Bfs traversal (top down, left to right).
		void bfs(std::shared_ptr<Node> node) const
		{
			Queue<std::shared_ptr<Node>> q;

			if (node != nullptr)
			{
				q.enqueue(node);

				while (!q.empty())
				{
					node = q.front();
					q.dequeue();

					std::cout << node->data;

					if (node->left != nullptr)
						q.enqueue(node->left);

					if (node->right != nullptr)
						q.enqueue(node->right);
				}
			}
		}

#ifdef BALANCE_TREE
		// Return minimum value of either child nodes.
		T min(std::shared_ptr<Node> node) const
		{
			if (node->isLeaf())
				return node->data;

			return node->left ? min(node->left) : min(node->right);
		}

		// Remove first instance of data from tree.
		bool remove(std::shared_ptr<Node>& node, T data)
		{
			if (!node)
				return false;
			else
			{
				if (node->data == data)
				{
					// No children.
					if (node->isLeaf())
						node.reset();
					else
					{
						// Node has 2 children.
						if (node->left && node->right)
						{
							T dataReplace = min(node->right);

							remove(dataReplace);
							node->data = dataReplace;
						}
						else
						{
							// Node has 1 child.
							std::shared_ptr<Node> tempPtr;

							node->left ? tempPtr = node->left : tempPtr = node->right;
							node.reset();   // Decrement use count.
							node = tempPtr; // Replace with only child.
						}
					}

					return true;
				}

				return remove(node->left, data) || remove(node->right, data);
			}
		}

		// Get height of node. Used by isBalanced function.
		static int getHeight(std::shared_ptr<Node> node)
		{
			return node == nullptr ? 0 : std::max(getHeight(node->left), getHeight(node->right)) + 1;
		}

		// Check if tree is balanced.
		static bool isBalanced(std::shared_ptr<Node> node)
		{
			if (node == nullptr)
				return true;

			int left = getHeight(node->left);
			int right = getHeight(node->right);

			return (abs(left - right) <= 1 && isBalanced(node->left) && isBalanced(node->right));
		}

		// Balance tree helper method, builds tree from (sorted) array of data elements.
		void buildTree(Vector<T>& data, int start, int end)
		{
			if (start <= end)
			{
				int mid = (start + end) / 2;

				add(data[mid]);

				buildTree(data, start, mid - 1);
				buildTree(data, mid + 1, end);
			}
		}

		// Balance tree helper method, constructs sorted array of tree data via inOrder traversal.
		void makeArray(std::shared_ptr<Node> node, Vector<T>& data)
		{
			// Base case.
			if (!node)
				return;

			// Store tree nodes inorder.
			makeArray(node->left, data);
			data.push_back(node->data);
			makeArray(node->right, data);
		}

		// Attempt to reconstruct tree as balanced. 
		void balanceTree(std::shared_ptr<Node> node)
		{
			// Store nodes in sorted order.
			Vector<T> data;
			makeArray(node, data);
			// Reconstruct a balanced tree.
			clear(root);
			// Static cast required for x64 because size_t == 64-bits.
			buildTree(data, 0, static_cast<int>(data.size() - 1));
		}
#endif // End BALANCE_TREE.
	};
}

#endif
