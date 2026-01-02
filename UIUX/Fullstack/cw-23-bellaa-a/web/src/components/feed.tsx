import Header from "./header";
import { useStore } from "@nanostores/react";
import { $showAddComment } from "@/lib/store"; // 👀 Look here
//import AddPost from "../post/add-post";
//import Posts from "../post/posts";
import AddComment from "../components/comments/add-comment"; // 👀 Look here
import Comments from "../components/comments/comments"; // 👀 Look here

const Feed = () => {
  //const showNewPostEditor = useStore($showAddPost);
  const showNewCommentEditor = useStore($showAddComment); // 👀 Look here

  return (
    <div className="flex flex-col w-full min-h-screen border-x">
      <Header />
      {/* showNewPostEditor && <AddPost /> */}
      {/* <Posts /> */}
      {/* 👆 Look here 👇 */}
      {showNewCommentEditor && <AddComment postId="1" />}
      <Comments postId="1" />
    </div>
  );
};

export default Feed;