import { toast } from "@/components/ui/use-toast";
import { createPost, deletePost, editPost } from "@/data/api";
import { addPost, removePost, updatePostContent } from "@/lib/store";

function useMutationPosts() {
  const deletePostById = async (postId: string) => {
    await deletePost(postId);
    removePost(postId);
  };

  const addNewPost = async (content: string) => {
    try {
      const newPost = await createPost(content);
      addPost(newPost);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error adding a new post 🙁",
        description: errorMessage,
      });
    }
  };

  const updatePost = async (postId: string, content: string) => {
    try {
      const updatedPost = await editPost(postId, content);
      updatePostContent(updatedPost.id, updatedPost.content);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error updating the post 🙁",
        description: errorMessage,
      });
    }
  };

  return {
    deletePostById,
    addNewPost,
    updatePost,
  };
}

export default useMutationPosts;
