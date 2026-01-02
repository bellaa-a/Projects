  import AddPost from "./add-post";
  import Header from "./header";
  import Posts from "./posts";

  type FeedProps = {
    showAddPost: boolean;
    setShowAddPost: React.Dispatch<React.SetStateAction<boolean>>;
  };

  const Feed = ({ showAddPost, setShowAddPost }: FeedProps) => {

    return (
      <div className="flex flex-col w-full min-h-screen border-x">
        <Header />
        {showAddPost && (
        <AddPost setShowAddPost={setShowAddPost} />
        )}
      <Posts />
      </div>
    );
  };

  export default Feed;