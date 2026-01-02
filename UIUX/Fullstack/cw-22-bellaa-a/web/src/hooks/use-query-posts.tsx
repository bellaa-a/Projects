import { useEffect } from "react";
import { fetchPosts } from "@/data/api";
import { useStore } from "@nanostores/react";
import { setPosts, $posts } from "@/lib/store";
import { toast } from "@/components/ui/use-toast";

function useQueryPosts() {
  const posts = useStore($posts);

  const loadPosts = async () => {
    try {
      const fetchedPosts = await fetchPosts();
      setPosts([...fetchedPosts]);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error reading the posts 🙁",
        description: errorMessage,
      });
    }
  };

  useEffect(() => {
    loadPosts();
  }, []);

  return { posts };
}

export default useQueryPosts;
