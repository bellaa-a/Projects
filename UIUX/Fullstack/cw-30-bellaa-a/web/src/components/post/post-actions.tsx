import { Button } from "@/components/ui/button";
import { PostType } from "@/data/types";
import { Pencil2Icon, ChatBubbleIcon } from "@radix-ui/react-icons";
import DeletePostDialog from "./delete-post-dialog";
import { openPage } from "@nanostores/router"; // 👀 Look here
import { $router } from "@/lib/router"; // 👀 Look here
import useAuth from "@/hooks/use-auth";
import { toast } from "../ui/use-toast";

const PostActions = ({
  post,
  setIsEditing,
}: {
  post: PostType;
  setIsEditing: (flag: boolean) => void;
}) => {

  const { user } = useAuth();

  const authGuard = () => {
    if (user && user.username) {
      return true;
    }
    toast({
      variant: "destructive",
      title: "Sorry! You need to sign in 🙁",
      description: "To view the comments, you must register and sign in.",
    });
    return false;
  }

  const ownershipGuard = () => {
    if (user && user.id && user.id === post.author.id) {
      return true;
    }
    return false;
  }
  
  const navigateToCommentsView = (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    authGuard() && openPage($router, "post", { postId: post.id });
  };

  return (
    <div className="flex justify-end">
      <Button variant={"ghost"} size={"icon"} onClick={navigateToCommentsView}>
        <ChatBubbleIcon className="w-4 h-4" />
      </Button>
      
      { ownershipGuard() && <Button
        variant={"ghost"}
        size={"icon"}
        onClick={() => setIsEditing(true)}
      >
        <Pencil2Icon className="w-4 h-4" />
      </Button> }
      { ownershipGuard() && <DeletePostDialog postId={post.id} /> }
    </div>
  );
};

export default PostActions;