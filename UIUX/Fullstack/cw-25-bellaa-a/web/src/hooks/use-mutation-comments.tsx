import { toast } from "@/components/ui/use-toast";
import { createComment, deleteComment, editComment } from "@/data/api";
import { addComment, removeComment, updateCommentContent } from "@/lib/store";

function useMutationComments(postId: string) {
  const deleteCommentById = async (commentId: string) => {
    try {
      await deleteComment(postId, commentId);
      removeComment(commentId);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error deleting the comment 🙁",
        description: errorMessage,
      });
    }
  };

  const addNewComment = async (content: string) => {
    try {
      const newComment = await createComment(postId, content);
      addComment(newComment);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error adding a new comment 🙁",
        description: errorMessage,
      });
    }
  };

  const updateComment = async (commentId: string, content: string) => {
    try {
      const updatedComment = await editComment(postId, commentId, content);
      updateCommentContent(updatedComment.id, updatedComment.content);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error updating the comment 🙁",
        description: errorMessage,
      });
    }
  };

  return {
    deleteCommentById,
    addNewComment,
    updateComment,
  };
}

export default useMutationComments;
