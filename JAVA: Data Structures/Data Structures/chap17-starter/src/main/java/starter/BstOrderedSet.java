package starter;

import java.util.Iterator;
import java.util.Queue;
import java.util.ArrayDeque;
import java.util.Stack;

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

  /**
   * Driver program to visualize/test this implementation.
   *
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    BstOrderedSet<Integer> bst = new BstOrderedSet<>();
    bst.insert(7);
    bst.insert(2);
    bst.insert(13);
    bst.insert(4);
    bst.insert(10);
    bst.insert(15);
    bst.insert(5);
    bst.insert(8);
    bst.insert(11);
    bst.insert(14);
    bst.insert(17);

    bst.remove(5);
    bst.remove(2);
    bst.remove(13);

    bst.printInOrder();

    for (int x : bst) {
      System.out.print(x +  " ");
    }
  }

  /* recursive find */
  private Node<T> find(Node<T> node, T data) {
    if (node == null) {
      return null;
    }
    if (node.data.compareTo(data) > 0) {
      return find(node.left, data);
    } else if (node.data.compareTo(data) < 0) {
      return find(node.right, data);
    } else {
      return node;
    }
  }

  @Override
  public void insert(T t) {
    // Uses a recursive (private) helper insert
    root = insert(root, t);
  }

  /* inserts given value into subtree rooted at given node
      and returns changed subtree with new node added. */
  private Node<T> insert(Node<T> node, T data) {
    if (node == null) {
      /* If the subtree is empty, return a new node */
      numElements++;
      return new Node<>(data);
    }

    /* Otherwise, recur down the tree */
    if (node.data.compareTo(data) > 0) {
      node.left = insert(node.left, data);
    } else if (node.data.compareTo(data) < 0) {
      node.right = insert(node.right, data);
    }

    /* return the (unchanged) node pointer */
    return node;
  }

  @Override
  public void remove(T t) {
    // TODO Implement me!
  }

  @Override
  public boolean has(T t) {
    return find(root, t) != null;
  }

  @Override
  public int size() {
    return numElements;
  }

  public void printInOrder() {
    printInOrder(root);
    System.out.println();
  }

  private void printInOrder(Node<T> node) {
    // if node is null --> do nothing
    if (node != null) {
      printInOrder(node.left);
      System.out.print(node.data + " ");
      printInOrder(node.right);
    }
  }

  public void printPreOrder() {
    printPreOrder(root);
    System.out.println();
  }

  private void printPreOrder(Node<T> node) {
    // if node is null --> do nothing
    if (node != null) {
      System.out.print(node.data + " ");
      printPreOrder(node.left);
      printPreOrder(node.right);
    }
  }

  public void printPostOrder() {
    printPostOrder(root);
    System.out.println();
  }

  private void printPostOrder(Node<T> node) {
    // if node is null --> do nothing
    if (node != null) {
      printPostOrder(node.right);
      printPostOrder(node.left);
      System.out.print(node.data + " ");
    }
  }

  @Override
  public Iterator<T> iterator() {
    // TODO Implement me!
    Queue<T> queue = new ArrayDeque<>();
    fillQueueInOrder(root, queue);
    return queue.iterator();
    return null;
  }

  private void fillQueueInOrder(Node<T> node, Queue<T> queue) {
    if (node != null) {
      fillQueueInOrder(node.left, queue);
      queue.add(node.data);
      fillQueueInOrder(node.right, queue);
    }
  }

  private class InorderIterator implements Iterator<T> {
    private final Stack<Node<T>> stack;

    inorderIterator() {
      stack = new Stack<>();
      pushLeft(root); //avg O(lg n); worse O(n)
    }

    private void pushLeft(Node<T> node) {

    }
  }


  private int height(Node<T> node) {
    if (node == null) {
      return -1;
    } else {
      return 1 + Math.max(height(node.left), height(node.right));
    }
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
