import { HomeIcon, MagnifyingGlassIcon, PlusCircledIcon } from "@radix-ui/react-icons";
import { Button } from "./ui/button";

const Sidebar = () => {
    return (
        <div className="flex flex-col space-y-2 p-2 items-end">
            <Button variant="ghost" size="icon">
                <HomeIcon className="w-5 h-5" />
            </Button>
            <Button variant={"ghost"} size="icon">
                <MagnifyingGlassIcon className="w-5 h-5" />
            </Button>
            <Button size="icon">
                <PlusCircledIcon className="w-5 h-5" />
            </Button>
        </div>
    );
}

export default Sidebar;