import Header from "./header";
import { useStore } from "@nanostores/react";
import { $showAddPost, $showAddComment } from "@/lib/store"; // 👀 Look here
import AddPost from "../post/add-post";
import Posts from "../post/posts";
import AddComment from "../comment/add-comment"; // 👀 Look here
import Comments from "../comment/comments"; // 👀 Look here

const Feed = ({ postId }: { postId?: string  }) => { // 👀 Look here
  const showNewPostEditor = useStore($showAddPost);
  const showNewCommentEditor = useStore($showAddComment);
 
  if (!postId) {
    return (
      <div className="flex flex-col w-full min-h-screen border-x">
        <Header />
        {showNewPostEditor && <AddPost />}
        <Posts />
      </div>
    );
  }
 
  // 👆 Look here 👇
 
  return (
    <div className="flex flex-col w-full min-h-screen border-x">
      <Header />
      {showNewCommentEditor && <AddComment postId={postId} />}
      <Comments postId={postId} />
    </div>
  );
};

export default Feed;
