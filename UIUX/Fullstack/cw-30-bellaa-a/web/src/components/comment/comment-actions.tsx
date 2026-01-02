import { Button } from "@/components/ui/button";
import { CommentType } from "@/data/types";
import { Pencil2Icon } from "@radix-ui/react-icons";
import DeleteCommentDialog from "./delete-comment-dialog";
import useAuth from "@/hooks/use-auth";

const CommentActions = ({
  comment,
  setIsEditing,
}: {
  comment: CommentType;
  setIsEditing: (flag: boolean) => void;
}) => {

  const { user } = useAuth();

  const ownershipGuard = () => {
    if (user && user.id && user.id === comment.author.id) {
      return true;
    }
    return false;
  }

  if (!ownershipGuard()) {
    return null;
  }
  
  return (
    <div className="flex justify-end">
      <Button
        variant={"ghost"}
        size={"icon"}
        onClick={() => setIsEditing(true)}
      >
        <Pencil2Icon className="w-4 h-4" />
      </Button>
      <DeleteCommentDialog commentId={comment.id} postId={comment.postId} />
    </div>
  );
};

export default CommentActions;
