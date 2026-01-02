package exception;

/**
 * Exception for invalid index. Data structures using (integer) indices
 * throw IndexException if a given index is out of range.
 */
public class IndexException extends RuntimeException {

  /**
   * Constructs a new IndexException.
   */
  public IndexException() {
  }

  /**
   * Constructs a new IndexException with the specified detail message.
   *
   * @param message the detail message. The detail message is saved for
   *                later retrieval by the getMessage() method.
   */
  public IndexException(String message) {
    super(message);
  }
}
