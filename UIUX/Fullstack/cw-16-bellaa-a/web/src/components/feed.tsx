import Header from "./header";
import Posts from "./posts";

const Feed = () => {
  return (
    <div className="flex flex-col w-full min-h-screen border-x">
      <Header />
      <Posts />
    </div>
  );
};

export default Feed;
