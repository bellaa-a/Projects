import {
  HomeIcon,
  MagnifyingGlassIcon,
  PlusCircledIcon,
} from "@radix-ui/react-icons";
import { Button } from "@/components/ui/button";
import { useStore } from "@nanostores/react";
import { $showAddPost, toggleAddPost } from "@/lib/store";

const Sidebar = () => {
  const showAddPost = useStore($showAddPost);

  return (
    <div className="flex flex-col items-end p-2 space-y-2">
      <Button aria-label={"Home"} variant="ghost" size="icon">
        <HomeIcon className="w-5 h-5" />
      </Button>
      <Button aria-label={"Search"} variant="ghost" size="icon">
        <MagnifyingGlassIcon className="w-5 h-5" />
      </Button>
      {!showAddPost && (
        <Button
          aria-label={"Make a Post"}
          variant="default"
          size="icon"
          onClick={() => {
            toggleAddPost();
          }}
        >
          <PlusCircledIcon className="w-5 h-5" />
        </Button>
      )}
    </div>
  );
};

export default Sidebar;
