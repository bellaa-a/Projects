import Sidebar from "@/components/sidebar";
import Feed from "./components/feed";

function App() {
  return (
    <div className="flex min-h-dvh">
      <div className="flex-1 min-w-14">
        <Sidebar />
      </div>
      <div className="w-full max-w-md mx-auto md:max-w-lg">
        <Feed />
      </div>
      <div className="flex-1 min-w-14">
        {/* Placeholder for another sidebar */}
      </div>
    </div>
  );
}

export default App;
