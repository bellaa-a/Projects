package exceptions;

/**
 * Exception for invalid length. Data structures that have a fixed (integer)
 * length throw LengthException if a given length is out of range.
 */
public class LengthException extends RuntimeException {
  /**
   * Constructs a new LengthException.
   */
  public LengthException() {
  }

  /**
   * Constructs a new LengthException with the specified detail message.
   *
   * @param message the detail message. The detail message is saved for
   *                later retrieval by the {@link #getMessage()} method.
   */
  public LengthException(String message) {
    super(message);
  }
}
