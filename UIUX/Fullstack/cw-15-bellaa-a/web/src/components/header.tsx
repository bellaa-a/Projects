import { Button } from "./ui/button";


const Header = () => {

    return (
        <div className="flex justify-center gap-3 p-1 border-b-4 border-stone-400">
            <Button variant="link">My Posts</Button>
            <Button variant="link" disabled={true}>All Posts</Button>
        </div>
    );
}

export default Header;