import { useEffect, useState } from "react";
import { Label } from "@/components/ui/label";
import { Textarea } from "@/components/ui/textarea";
import { Button } from "@/components/ui/button";
import { CommentType } from "@/data/types";
import { useToast } from "@/components/ui/use-toast";
import useMutationComment from "@/hooks/use-mutation-comments";

const EditComment = ({
  comment,
  setIsEditing,
}: {
  comment: CommentType;
  setIsEditing: (flag: boolean) => void;
}) => {
  const [id, setId] = useState("");
  const [postId, setPostId] = useState("");
  const [content, setContent] = useState("");
  const { updateComment } = useMutationComment(postId);
  const { toast } = useToast();

  useEffect(() => {
    if (comment) {
      comment.id !== id && setId(comment.id);
      comment.content !== content && setContent(comment.content);
      comment.postId !== postId && setPostId(comment.postId);
    }

    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [comment]);

  const cleanUp = () => {
    setIsEditing(false);
  };

  const saveComment = async () => {
    if (!content) {
      toast({
        variant: "destructive",
        title: "Sorry! Comment cannot be empty! 🙁",
        description: `Please enter the content of your comment.`,
      });
    } else {
      await updateComment(id, content);
      cleanUp();
    }
  };

  const handleSave = async (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    saveComment();
  };

  const handleSaveOnEnter = (e: React.KeyboardEvent<HTMLTextAreaElement>) => {
    if (e.key === "Enter") {
      e.preventDefault();
      saveComment();
    }
  };

  const handleCancel = () => {
    cleanUp();
  };

  return (
    <form className="grid w-full gap-1.5 p-4 border-b">
      <Label htmlFor="content">Edit your comment</Label>
      <Textarea
        id="content"
        placeholder="Type your comment here."
        value={content}
        onKeyDown={handleSaveOnEnter}
        onChange={(e) => setContent(e.target.value)}
      />
      <div className="flex justify-end gap-3">
        <Button type="reset" variant={"secondary"} onClick={handleCancel}>
          Cancel
        </Button>
        <Button type="submit" onClick={handleSave}>
          Post
        </Button>
      </div>
    </form>
  );
};

export default EditComment;
