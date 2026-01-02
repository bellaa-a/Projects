import Header from "./header";
import Posts from "./posts";

const Feed = () => {

    return (
        <div className="flex flex-col space-y-2">
            <Header />
            <Posts />
        </div>
    );
}

export default Feed;