import {
    CovidData,
    DiseaseApiResponse,
    PartialCountryData,
    CountryData,
    DiseaseCountryInfo,
  } from "./types.ts";
  
  const REST_DISEASE_API = "https://disease.sh/v3/covid-19/countries";
  const REST_COUNTRIES_API = "https://restcountries.com/v3.1/all";
  
  // Fetch the COVID-19 data for the given country code
  // The country code is the ISO 3166-1 alpha-2 code
  export async function fetchCovidData(countryCode: string): Promise<CovidData> {
    const response = await fetch(`${REST_DISEASE_API}/${countryCode}`);
    if (!response.ok) {
      throw new Error(`API request failed! with status: ${response.status}`);
    }
    const data: DiseaseApiResponse = await response.json();
    const countryInfo: DiseaseCountryInfo = data.countryInfo;
  
    // Parse and return the required data
    return {
      countryName: data.country,
      countryCode: countryInfo.iso2,
      countryFlag: countryInfo.flag,
      confirmed: data.cases,
      active: data.active,
      recovered: data.recovered,
    };
  }
  
  // Fetch the list of countries from the REST Countries API
  export async function fetchCountries(): Promise<CountryData[]> {
    const response = await fetch(REST_COUNTRIES_API);
    if (!response.ok) {
      throw new Error(`API request failed! with status: ${response.status}`);
    }
    const data: PartialCountryData[] = await response.json();
  
    // Map the API data to the required format
    return data.map((country) => ({
      name: country.name.common,
      code: country.cca2,
    }));
  }