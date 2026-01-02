import { PostType } from "@/data/types";
import PostActions from "./post-actions";

const Post = ({ post }: { post: PostType }) => {
  return (
    <div className="p-1 border-b">
      <div className="flex items-center justify-between pl-4">
        <div className="text-xs text-muted-foreground">
          {new Date(post.date).toLocaleString("en-US", {
            month: "short",
            day: "numeric",
            year: "numeric",
            hour: "numeric",
            minute: "numeric",
          })}
        </div>
        <PostActions />
      </div>
      <p className="p-4">{post.content}</p>
    </div>
  );
};

export default Post;
