import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import { Avatar, AvatarFallback, AvatarImage } from "@/components/ui/avatar";
import { useEffect, useState } from "react";
import { CovidData } from "@/data/types";
import { fetchCovidData } from "@/data/api";

const STATISTICS: (keyof CovidData)[] = ["confirmed", "active", "recovered"];

type DisplayStatisticsProps = {
  countryCode: string | null;
};

const DisplayStatistics = ({ countryCode }: DisplayStatisticsProps) => {
  const [covidData, setCovidData] = useState<CovidData | null>(null);

  useEffect(() => {
    console.log("countryCode", countryCode);
    if (countryCode) {
      fetchCovidData(countryCode).then((data) => setCovidData(data));
    }
  }, [countryCode]);

  if (!countryCode) {
    return null;
  }

  return (
    <div className="flex w-full justify-between flex-col sm:flex-row gap-5">
      {STATISTICS.map((statistic) => (
        <Card className="w-full" key={statistic}>
          <CardHeader>
            <CardTitle className="capitalize">{statistic}</CardTitle>
          </CardHeader>
          <CardContent className="flex justify-right items-center gap-2">
            <Avatar>
              <AvatarImage src={covidData?.countryFlag} />
              <AvatarFallback>
                {covidData?.countryCode.toUpperCase()}
              </AvatarFallback>
            </Avatar>
            <div className="text-2xl">
              {covidData?.[statistic]?.toLocaleString()}
            </div>
          </CardContent>
        </Card>
      ))}
    </div>
  );
};

export default DisplayStatistics;