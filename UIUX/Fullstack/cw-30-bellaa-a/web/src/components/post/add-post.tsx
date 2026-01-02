import { useState } from "react";
import { Textarea } from "@/components/ui/textarea";
import { Button } from "@/components/ui/button";
import { Label } from "@/components/ui/label";
import { toggleAddPost } from "@/lib/store";
import useMutationPosts from "@/hooks/use-mutation-posts";
import { useToast } from "@/components/ui/use-toast";

const AddPost = () => {
  const [content, setContent] = useState("");
  const { addNewPost } = useMutationPosts();
  const { toast } = useToast();

  const cleanUp = () => {
    setContent("");
    toggleAddPost();
  };

  const savePost = async () => {
    if (!content) {
      toast({
        variant: "destructive",
        title: "Sorry! Content cannot be empty! 🙁",
        description: `Please enter the content of your post.`,
      });
    } else {
      await addNewPost(content);
      cleanUp();
    }
  };

  const handleSave = async (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    savePost();
  };

  const handleSaveOnEnter = (e: React.KeyboardEvent<HTMLTextAreaElement>) => {
    if (e.key === "Enter") {
      e.preventDefault();
      savePost();
    }
  };

  const handleCancel = () => {
    cleanUp();
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

export default AddPost;
