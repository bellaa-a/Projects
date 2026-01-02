import { toast } from "@/components/ui/use-toast";
import { signIn, signOut, signUp } from "@/data/api";
import { $user, clearUser, setUser } from "@/lib/store";
import { useStore } from "@nanostores/react";

function useAuth() {
  const user = useStore($user);

  const login = async (username: string, password: string) => {
    try {
      if (!username || !password) {
        throw new Error("Username and password are required!");
      }
      // call the backend
      const user = await signIn(username, password);
      // updte the store
      setUser(user);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error signing you in🙁",
        description: errorMessage,
      });
    }
  }

  const logout = async () => {
    try {
      // call the backend
      const success = await signOut();
      // updte the store
      success && clearUser();
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error signing you out 🙁",
        description: errorMessage,
      });
    }
  }

  const register = async (name:string, username: string, password: string) => {
    try {
      if (!name || !username || !password) {
        throw new Error("Name and username and password are required!");
      }
      // call the backend
      const user = await signUp(name, username, password);
      // updte the store
      setUser(user);
    } catch (error) {
      const errorMessage =
        (error as Error).message ?? "Please try again later!";
      toast({
        variant: "destructive",
        title: "Sorry! There was an error signing you up 🙁",
        description: errorMessage,
      });
    }
  }

  return {
    user,
    login,
    logout,
    register
  }
}

export default useAuth;