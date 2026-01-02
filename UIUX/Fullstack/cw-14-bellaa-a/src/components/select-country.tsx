import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";
import { Label } from "@/components/ui/label";
import { useEffect, useState } from "react";
import { CountryData } from "@/data/types";
import { fetchCountries } from "@/data/api";

type SelectCountryProps = {
  setCountryCode: (countryCode: string | null) => void;
};

const SelectCountry = ({ setCountryCode }: SelectCountryProps) => {
  const [country, setCountry] = useState<CountryData | null>(null);
  const [countryData, setCountryData] = useState<CountryData[]>([]);

  useEffect(() => {
    fetchCountries().then((data) => setCountryData(data));
  }, []);

  const handleOnCountryChange = (value: string) => {
    const country = countryData.find((country) => country.code === value);
    setCountry(country!);
    setCountryCode(value);
  };

  return (
    <div className="w-full space-y-12">
      <h1 className="text-6xl">Covid Statistics</h1>
      <div className="flex flex-col gap-5 justify-start w-full">
        <Label className="text-xl">Select a country:</Label>
        <Select value={country?.code} onValueChange={handleOnCountryChange}>
          <SelectTrigger className="w-full text-xl p-8 bg-white">
            <SelectValue placeholder="Country..." />
          </SelectTrigger>
          <SelectContent>
            {countryData.map((country) => (
              <SelectItem value={country.code} key={country.code}>
                {country.name}
              </SelectItem>
            ))}
          </SelectContent>
        </Select>
      </div>
    </div>
  );
};

export default SelectCountry;