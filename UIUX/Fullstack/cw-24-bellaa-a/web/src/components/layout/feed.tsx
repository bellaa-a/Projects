// web/src/components/layout/feed.tsx
import Header from "./header";
import { useStore } from "@nanostores/react";
import { $showAddPost, $showAddComment } from "@/lib/store";
import AddPost from "@/components/post/add-post";
import Posts from "@/components/post/posts";
import AddComment from "@/components/comment/add-comment";
import Comments from "@/components/comment/comments";

const Feed = ({ postId }: { postId: string | null }) => { // 👀 Look here
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