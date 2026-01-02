import { Button } from "@/components/ui/button";
import { PostType } from "@/data/types";
import { Pencil2Icon } from "@radix-ui/react-icons";
import DeletePostDialog from "./delete-post-dialog";

type PostActionsProps = {
  post: PostType;
  setIsEditing: React.Dispatch<React.SetStateAction<boolean>>;
};

const PostActions = ({ post, setIsEditing }: PostActionsProps) => {  return (
    <div className="flex justify-end">
      <Button
        variant={"ghost"}
        size={"icon"}
        onClick={() => setIsEditing(true)}
      >
        <Pencil2Icon className="w-4 h-4" />
      </Button>
      <DeletePostDialog post={post} />    </div>
  );
};

export default PostActions;