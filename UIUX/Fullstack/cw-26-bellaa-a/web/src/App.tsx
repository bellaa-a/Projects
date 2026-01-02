import Feed from "@/components/layout/feed";
import Sidebar from "@/components/layout/sidebar";
import { Toaster } from "@/components/ui/toaster";
import { useStore } from "@nanostores/react";
import { $router } from "./lib/router";

function App() {
  const page = useStore($router); // 👀 Look here
  
  if (!page) {
    // 👀 Look here
    return (
      <div className="flex items-center justify-center min-h-dvh">
        404 Not Found
      </div>
    );
  }
 
  return (
    <div className="flex min-h-dvh">
      <div className="flex-1 min-w-14">
        <Sidebar />
      </div>
      <div className="w-full max-w-md mx-auto md:max-w-lg">
        {/* 👇 Look here 👇 */}
        {page.route === "home" && <Feed postId={null} />}
        {page.route === "post" && <Feed postId={page.params.postId} />}
      </div>
      <div className="flex-1">{/* Placeholder for another sidebar */}</div>
      <Toaster />
    </div>
  );
}

export default App;
