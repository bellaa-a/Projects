import { useEffect, useState } from "react";
import { Textarea } from "@/components/ui/textarea";
import { Button } from "@/components/ui/button";
import { Label } from "@/components/ui/label";
import { PostType } from "@/data/types";
import useMutationPosts from "@/hooks/use-mutation-posts";

type EditPostProps = {
  post: PostType;
  setIsEditing: React.Dispatch<React.SetStateAction<boolean>>;
};

const EditPost = ({ post, setIsEditing }: EditPostProps) => {
  const [id, setId] = useState("");
  const [content, setContent] = useState("");
  const { updatePost } = useMutationPosts();

  useEffect(() => {
    if (post && post.id !== id && post.content !== content) {
      setId(post.id);
      setContent(post.content);
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [post]);

  const handleTextChange = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
    setContent(e.target.value);
  };

  const handleSave = async (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
      updatePost(id, content);
      setContent("");
      setIsEditing(false);
  };

  const handleCancel = () => {
    setContent("");
    setIsEditing(false);
  };

  return (
    <form className="grid w-full gap-1.5 p-4 border-b">
      <Label htmlFor="content" className="text-sm">
        Your post
      </Label>
      <Textarea
        id="content"
        placeholder="Type your message here."
        value={content}
        onChange={handleTextChange}
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

export default EditPost;