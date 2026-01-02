import useAuth from "@/hooks/use-auth";
import { Button } from "../ui/button";
import { openPage } from "@nanostores/router";
import { $router } from "@/lib/router";

const UserMenu = () => {
  const { user, logout } = useAuth();

  const navigateToLogin = (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    openPage($router, "login");
  };

  if (!user.name) {
    return (
      <div className="m-3 space-y-2">
        <div className="text-sm font-medium text-foreground">
          {`Welcome to Posts!`}
        </div>
        <Button variant={"default"} onClick={navigateToLogin}>
          Sign in
        </Button>
      </div>
    );
  }

  return (
    <div className="m-3 space-y-2">
      <div className="text-sm font-medium text-foreground">
        {`Welcome ${user.name}!`}
      </div>
      <Button variant={"secondary"} onClick={logout}>
        Sign out
      </Button>
    </div>
  );
};

export default UserMenu;
