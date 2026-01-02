import { Button } from "@/components/ui/button";
import { TrashIcon, Pencil2Icon } from "@radix-ui/react-icons";

const PostActions = () => {
  return (
    <div className="flex justify-end">
      <Button variant={"ghost"} size={"icon"}>
        <Pencil2Icon className="w-4 h-4" />
      </Button>
      <Button variant={"ghost"} size={"icon"}>
        <TrashIcon className="w-4 h-4" />
      </Button>
    </div>
  );
};

export default PostActions;
