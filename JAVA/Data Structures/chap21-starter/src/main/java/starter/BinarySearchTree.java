package starter;

/**
 * A minimal (regular) BST.
 * You can only insert values in this BST.
 * You can ask for the height of this BST or whether it is balanced.
 */
public class BinarySearchTree {
  private Node root;

  private static class Node {
    int data;
    Node left;
    Node right;

    Node(int data) {
      this.data = data;
    }
  }

  /**
   * Insert a value in this BST.
   * @param data value to be inserted.
   *             Pre: data != null.
   */
  public void insert(int data) {
    root = insert(root,data);
  }

  // recursive helper insert
  private Node insert(Node node, int data) {
    if (node == null) {
      return new Node(data);
    } else if (node.data > data) {
      node.left = insert(node.left, data);
    } else if (node.data < data) {
      node.right = insert(node.right, data);
    }
    return node;
  }

  /**
   * Get the height of this tree.
   * @return height of this tree.
   */
  public int getHeight() {
    return height(root);
  }

  // Modified definition of height
  private int height(Node node) {
    if (node == null) {
      return -1;
    } else if (isLeaf(node)) {
      return 0;
    } else {
      return 1 + Math.max(height(node.left), height(node.right));
    }
  }

  private boolean isLeaf(Node node) {
    return node.left == null && node.right == null;
  }

  /**
   * Check if this tree is balanced.
   * @return true if this tree is balanced, false otherwise.
   */
  public boolean isBalanced() {
    return isBalanced(root);
  }

  // Goes over all the nodes (in-order traversal)
  //   checks if the node's balance factor is within range
  private boolean isBalanced(Node node) {
    if (node == null) {
      return true;
    }
    if (!isBalanced(node.left)) {
      return false;
    }
    int value = bf(node);
    if (value > 1 || value < -1) {
      return false;
    }
    return isBalanced(node.right);
  }

  // Return balance factor of the given node
  private int bf(Node node) {
    return height(node.left) - height(node.right);
  }
}
