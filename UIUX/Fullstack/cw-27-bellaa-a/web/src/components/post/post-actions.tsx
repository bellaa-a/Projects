import { Button } from "@/components/ui/button";
import { PostType } from "@/data/types";
import { Pencil2Icon, ChatBubbleIcon } from "@radix-ui/react-icons";
import DeletePostDialog from "./delete-post-dialog";
import { openPage } from "@nanostores/router"; // 👀 Look here
import { $router } from "@/lib/router"; // 👀 Look here

const PostActions = ({
  post,
  setIsEditing,
}: {
  post: PostType;
  setIsEditing: (flag: boolean) => void;
}) => {
  // 👇 Look here
  const navigateToCommentsView = (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    openPage($router, "post", { postId: post.id });
  };

  return (
    <div className="flex justify-end">
      <Button variant={"ghost"} size={"icon"} onClick={navigateToCommentsView}>
        <ChatBubbleIcon className="w-4 h-4" />
      </Button>
      {/* 👆 Look here */}
      <Button
        variant={"ghost"}
        size={"icon"}
        onClick={() => setIsEditing(true)}
      >
        <Pencil2Icon className="w-4 h-4" />
      </Button>
      <DeletePostDialog postId={post.id} />
    </div>
  );
};

export default PostActions;