import { useState } from "react";
import { Textarea } from "@/components/ui/textarea";
import { Button } from "@/components/ui/button";
import { Label } from "@/components/ui/label";
import useMutationPosts from "@/hooks/use-mutation-posts";

type AddPostProps = {
  setShowAddPost: React.Dispatch<React.SetStateAction<boolean>>;
};

const AddPost = ({ setShowAddPost }: AddPostProps) => {
  const [content, setContent] = useState("");
  const { addNewPost } = useMutationPosts();

  const handleTextChange = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
    setContent(e.target.value);
  };

  const handleSave = async (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    addNewPost(content);
    setContent("");
    setShowAddPost(false); // Close the form
  };

  const handleCancel = () => {
    setContent("");
    setShowAddPost(false); // Close the form
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

export default AddPost;
