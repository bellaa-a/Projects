import { useEffect, useState } from "react";
import { fetchPosts } from "@/data/api";
import { useStore } from "@nanostores/react";
import {
  $posts,
  $showMine,
  appendPosts,
  incrementPage,
  setHasMorePosts,
  setPosts,
} from "@/lib/store";
import { toast } from "@/components/ui/use-toast";
import useAuth from "./use-auth";

function useQueryPosts() {
  const posts = useStore($posts);
  const [isLoading, setIsLoading] = useState(false);
  const showMine = useStore($showMine);
  const { user } = useAuth();

  const loadPosts = async (
    page: number = 1,
    limit: number = 10,
    username?: string
  ) => {
    setIsLoading(true);
    try {
      if (showMine) {
        username = user.username;
      }
      const { data: fetchedPosts, total } = await fetchPosts(page, limit, username);
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
  }, [showMine]);

  return { posts, loadPosts, isLoading };
}

export default useQueryPosts;