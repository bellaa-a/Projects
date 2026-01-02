import AddPost from "./add-post";
import Header from "./header";
import Posts from "./posts";
import { useStore } from "@nanostores/react";
import { $showAddPost } from "@/lib/store";

const Feed = () => {
  const showNewPostEditor = useStore($showAddPost);

  return (
    <div className="flex flex-col w-full min-h-screen border-x">
      <Header />
      {showNewPostEditor && <AddPost />}
      <Posts />
    </div>
  );
};

export default Feed;
