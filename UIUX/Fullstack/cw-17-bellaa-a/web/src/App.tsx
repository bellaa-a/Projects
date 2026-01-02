import Sidebar from "@/components/sidebar";
import Feed from "@/components/feed";
import { useState } from "react";
import { Toaster } from "./components/ui/toaster";

function App() {
  const [showAddPost, setShowAddPost] = useState<boolean>(false);

  return (
    <div className="flex min-h-dvh">
      <div className="flex-1 min-w-14">
        <Sidebar showAddPost={showAddPost} setShowAddPost={setShowAddPost} />
      </div>
      <div className="w-full max-w-md mx-auto md:max-w-lg">
        <Feed showAddPost={showAddPost} setShowAddPost={setShowAddPost} />
      </div>
      <div className="flex-1">{/* Placeholder for another sidebar */}</div>
      <Toaster />
    </div>
  );
}

export default App;