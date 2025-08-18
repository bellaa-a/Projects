package starter;

import java.util.Iterator;

/**
 * A Binary Search Tree implementation of OrderedSet ADT.
 *
 * @param <T> Element type.
 */
public class BstOrderedSet<T extends Comparable<T>> implements OrderedSet<T> {

  private Node<T> root;
  private int numElements;

  /**
   * Construct an empty BstOrderedSet.
   */
  public BstOrderedSet() {
    root = null;
    numElements = 0;
  }

  @Override
  public void insert(T t) {
    root = insert(root, t);
  }

  //Always fall as follows: subtree = insert(subtree, data);
  private Node<T> insert(Node<T> node, T t) {
    if (node == null) { //base case
      node = new Node<>(t);
      numElements++;
    } else {
      int comparisonResult = t.compareTo(node.data);
      if (comparisonResult < 0) {
        insert(node.left, t);
      } else if (comparisonResult > 0) {
        insert(node.right, t);
      } //else, comparisonResult == 0 --> element already present!
    }
    return node;
  }

  @Override
  public void remove(T t) {
    root = remove(root, t);
  }

  //Always call as follows: subtree = remove(subtree, data);
  private Node<T> remove(Node<T> node, T t) {
    if (node != null) { //if node == null, element isn't present --> do nothing!
      int comparisonResult = t.compareTo(node.data);
      if (comparisonResult < 0) {
        node.left = remove(node.left, t);
      } else if (comparisonResult > 0) {
        node.right = remove(node.right, t);
      } else { // comparisonResult == 0 --> found the element
        node = removeSubtreeNode(node);
      }
    }
    return node;
  }

  private Node<T> removeSubtreeNode(Node<T> node) {
    if (node.left == null) { // no children or only one child
      --numElements;
      node = node.right;
    } else if (node.right == null) {
      --numElements;
      node = node.left;
    } else {
      T smallest = getSmallestElement(node.right);
      node.data = smallest;
      node.right = remove (node.right, smallest); // takes care of numElements decrement
    }
    return node;
  }

  private T getSmallestElement(Node<T> node) {
    Node<T> smallestNode = node;
    while (smallestNode.left != null) {
      smallestNode = smallestNode.left;
    }
    return smallestNode.data;
  }

  @Override
  public boolean has(T t) {
    return find(root, t) != null;
  }

  //look for t in subtree rooted at node
  private Node<T> find(Node<T> node, T t) {
    if (node == null) { //empty tree
      return null;
    } else {
      int comparisonResult = t.compareTo(node.data);
      if (comparisonResult == 0) {
        return node;
      } else if (comparisonResult < 0) {
        return find(node.left, t);
      } else {
        return find(node.right, t);
      }
    }
  }

  @Override
  public int size() {
    return numElements;
  }

  @Override
  public Iterator<T> iterator() {
    // TODO Implement me!
    return null;
  }

  private static class Node<E> {
    E data;
    Node<E> left;
    Node<E> right;

    Node(E data) {
      this.data = data;
    }
  }



}
