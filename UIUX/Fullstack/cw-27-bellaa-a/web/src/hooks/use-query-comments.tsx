import { useEffect } from "react";
import { fetchComments } from "@/data/api";
import { useStore } from "@nanostores/react";
import { setComments, $comments } from "@/lib/store";
import { toast } from "@/components/ui/use-toast";

function useQueryComments(postId: string) {
  const comments = useStore($comments);

  const loadComments = async () => {
    try {
      const fetchedComments = await fetchComments(postId);
      setComments([...fetchedComments]);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error reading the comments 🙁",
        description: errorMessage,
      });
    }
  };

  useEffect(() => {
    loadComments();
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [postId]);

  return { comments };
}

export default useQueryComments;
