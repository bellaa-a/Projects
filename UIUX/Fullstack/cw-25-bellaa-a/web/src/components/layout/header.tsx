import { Button } from "@/components/ui/button";
import { $router } from "@/lib/router"; // 👀 Look here
import { useStore } from "@nanostores/react"; // 👀 Look here

const Header = () => {
  const page = useStore($router); // 👀 Look here

  if (!page) return null; // 👀 Look here

  if (page.route === "post") {
    return (
    <div className="flex justify-center gap-3 p-1 border-b">
      <Button variant={"link"}>My Comments</Button>
      <Button variant={"link"} disabled={true}>
        All Comments
      </Button>
    </div>
    );
  }

  // 👆 Look here 👇

  return (
    <div className="flex justify-center gap-3 p-1 border-b">
      <Button variant={"link"}>My Posts</Button>
      <Button variant={"link"} disabled={true}>
        All Posts
      </Button>
    </div>
  );
};

export default Header;