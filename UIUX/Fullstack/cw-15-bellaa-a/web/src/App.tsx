import Feed from "./components/feed";
import Sidebar from "./components/sidebar";


function App() {
  return (
    <div className="flex min-h-screen">
      <div className="flex-1">
        <Sidebar />
      </div>
      <div className="w-full border-l-4 border-r-4 border-stone-400 max-w-md">
        <Feed />
      </div>
      <div className="flex-1">{ /* another sidebar */}</div>
    </div>
  );
}

export default App;