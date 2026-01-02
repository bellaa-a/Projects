import SelectCountry from "./components/select-country";
import DisplayStatistics from "./components/display-statistics";
import { useState } from "react";

function App() {
  const [countryCode, setCountryCode] = useState<string | null>(null);

  return (
    <div className="flex flex-col justify-between items-center min-h-screen container space-y-4 max-w-4xl py-10">
      <SelectCountry setCountryCode={setCountryCode} />
      <DisplayStatistics countryCode={countryCode} />
    </div>
  );
}

export default App;