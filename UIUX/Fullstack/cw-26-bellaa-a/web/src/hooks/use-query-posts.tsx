import { useEffect, useState } from "react";
import { fetchPosts } from "@/data/api";
import { useStore } from "@nanostores/react";
import {
  $posts,
  appendPosts,
  incrementPage,
  setHasMorePosts,
  setPosts,
} from "@/lib/store";
import { toast } from "@/components/ui/use-toast";

function useQueryPosts() {
  const posts = useStore($posts);
  const [isLoading, setIsLoading] = useState(false);

  const loadPosts = async (page: number = 1) => {
    setIsLoading(true);
    try {
      const { data: fetchedPosts, total } = await fetchPosts(page);
      setHasMorePosts(posts.length + fetchedPosts.length < total);
      if (page === 1) {
        setPosts(fetchedPosts);
      } else {
        appendPosts(fetchedPosts);
        incrementPage();
      }
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error reading the posts 🙁",
        description: errorMessage,
      });
    } finally {
      setIsLoading(false);
    }
  };

  useEffect(() => {
    loadPosts();
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  return { posts, loadPosts, isLoading };
}

export default useQueryPosts;