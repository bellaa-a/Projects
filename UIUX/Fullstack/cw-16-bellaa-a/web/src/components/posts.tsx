import { PostType } from "@/data/types";
import { useEffect, useState } from "react";
import Post from "./post";
import { fetchPosts } from "@/api/posts";

const Posts = () => {
  const [posts, setPosts] = useState<PostType[]>([]);

  useEffect(() => {
    fetchPosts()
      .then(data => setPosts(data))
      .catch(err => console.log(err))
  }, []) // it will run when this component is mounted 
  
  return (
    <>
      {posts
        .sort((post1, post2) => (post1.date > post2.date ? -1 : 1))
        .map((post) => (
          <Post key={post.id} post={post} />
        ))}
    </>
  );
};

export default Posts;
