// Define the types for the data that is returned from the https://disease.sh/
export interface DiseaseCountryInfo {
    _id: number;
    iso2: string;
    iso3: string;
    lat: number;
    long: number;
    flag: string;
  }
  
  // Define the types for the API response that is returned from the https://disease.sh/
  export interface DiseaseApiResponse {
    updated: number;
    country: string;
    countryInfo: DiseaseCountryInfo;
    cases: number;
    todayCases: number;
    deaths: number;
    todayDeaths: number;
    recovered: number;
    todayRecovered: number;
    active: number;
    critical: number;
    casesPerOneMillion: number;
    deathsPerOneMillion: number;
    tests: number;
    testsPerOneMillion: number;
    population: number;
    continent: string;
    oneCasePerPeople: number;
    oneDeathPerPeople: number;
    oneTestPerPeople: number;
    activePerOneMillion: number;
    recoveredPerOneMillion: number;
    criticalPerOneMillion: number;
  }
  
  // Define the types for the statistics that are displayed in the dashboard
  export interface CovidData {
    countryName: string;
    countryCode: string;
    countryFlag: string;
    confirmed: number;
    active: number;
    recovered: number;
  }
  
  // Define the types for the data that is returned from the https://restcountries.com/
  export interface PartialCountryData {
    name: {
      common: string;
      official: string;
    };
    cca2: string; // ISO 3166-1 alpha-2 code
    cca3: string; // ISO 3166-1 alpha-3 code
  }
  
  // Define the types for the dropdown that are displayed in the dashboard
  export interface CountryData {
    name: string;
    code: string; // ISO 3166-1 alpha-2 code
  }