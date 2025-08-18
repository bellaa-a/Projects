package exceptions;

/**
 * Exception for position-based data structures.
 *
 * <p>Data structures that use Position interface throw exceptions.PositionException
 * if the position provided to them is null or otherwise invalid.</p>
 */
public class PositionException extends RuntimeException {
  /**
   * Constructs a new exceptions.PositionException.
   */
  public PositionException() {

  }

  /**
   * Constructs a new exceptions.PositionException with the specified detail message.
   *
   * @param message the detail message. The detail message is saved for
   *                later retrieval by the getMessage() method.
   */
  public PositionException(String message) {
    super(message);
  }
}