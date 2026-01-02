import { Button } from "@/components/ui/button";

const Header = () => {
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