import { Button } from "@/components/ui/button";
import { CommentType } from "@/data/types";
import { Pencil2Icon } from "@radix-ui/react-icons";
import DeleteCommentDialog from "./delete-comment-dialog";

const CommentActions = ({
  comment,
  setIsEditing,
}: {
  comment: CommentType;
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
      <DeleteCommentDialog commentId={comment.id} postId={comment.postId} />
    </div>
  );
};

export default CommentActions;
