import { Button } from "@/components/ui/button";
import { PostType } from "@/data/types";
import { Pencil2Icon } from "@radix-ui/react-icons";
import DeletePostDialog from "./delete-post-dialog";

const PostActions = ({
  post,
  setIsEditing,
}: {
  post: PostType;
  setIsEditing: (flag: boolean) => void;
}) => {
  return (
    <div className="flex justify-end">
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
