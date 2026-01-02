import Post from "./post";
import useQueryPosts from "@/hooks/use-query-posts";

  const Posts = () => {
  const { posts } = useQueryPosts();


    return (
      <div>
        {posts
          .sort((a, b) => (a.date > b.date ? -1 : 1))
          .map((post) => (
            <Post key={post.id} post={post} />
          ))}
      </div>
    );
  };

export default Posts;