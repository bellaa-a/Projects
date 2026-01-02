import "../style.css";

const dictionaryAPI: string =
  "https://api.dictionaryapi.dev/api/v2/entries/en_US/";

interface Definition {
  definition: string;
  example?: string;
  synonyms?: string[];
  antonyms?: string[];
}

interface Meaning {
  partOfSpeech: string;
  definitions: Definition[];
}

interface Phonetic {
  text?: string;
  audio?: string;
}

interface DictionaryAPIResponse {
  word: string;
  phonetic?: string;
  phonetics: Phonetic[];
  origin?: string;
  meanings: Meaning[];
}

// Fetch data from the API
const searchWord = async (word: string): Promise<DictionaryAPIResponse[]> => {
  try {
    const response = await fetch(`${dictionaryAPI}${word}`);
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
    const data: DictionaryAPIResponse[] = await response.json();
    return data;
  } catch (error) {
    throw error; // Re-throw the error
  }
};

// Extract word definitions from the data
const extractWordDefinitions = (
  data: DictionaryAPIResponse[] | undefined,
): Meaning[] | undefined => {
  return data?.[0]?.meanings ?? undefined;
};

// Extract word phonetics from the data
const extractWordPhonetics = (
  data: DictionaryAPIResponse[] | undefined,
): Phonetic[] | undefined => {
  return data?.[0]?.phonetics ?? undefined;
};

// Display the word definitions
const displayWordDefinition = (meanings: Meaning[] | undefined): void => {
  const definitionsSection = document.getElementById(
    "definitions",
  ) as HTMLElement;
  definitionsSection.innerHTML = ""; // Clear previous content

  const definitionsHeading = `<h1 class="text-2xl font-semibold">Definitions</h1>`;
  definitionsSection.innerHTML += definitionsHeading;

  meanings?.forEach((meaning: Meaning) => {
    const definitionItems = meaning.definitions
      .map((def) => `<li>${def.definition}</li>`)
      .join("");
    const definitionBlock = `
      <div class="bg-sky-50">
        <p class="px-4 py-2 font-semibold text-white bg-sky-600">${meaning.partOfSpeech}</p>
        <ul class="p-2 ml-6 font-light list-disc text-sky-700">${definitionItems}</ul>
      </div>
    `;
    definitionsSection.innerHTML += definitionBlock;
  });
};

// Display the word phonetics
const displayWordPhonetic = (phonetics: Phonetic[] | undefined): void => {
  const phoneticsSection = document.getElementById("phonetics") as HTMLElement;
  phoneticsSection.innerHTML = ""; // Clear previous content
  phoneticsSection.classList.add("flex", "flex-col", "gap-4");

  const phoneticsHeading = `<h1 class="text-2xl font-semibold">Phonetics</h1>`;
  phoneticsSection.innerHTML += phoneticsHeading;

  phonetics?.forEach((phonetic: Phonetic) => {
    if (!phonetic.text || !phonetic.audio) return;

    const phoneticBlock = `
      <div class="bg-stone-100">
        <p class="px-4 py-3 text-white bg-stone-700">${phonetic.text}</p>
        <audio style="width: 100%" controls>
          <source src="${phonetic.audio}" type="audio/mpeg">
          Your browser does not support the audio element.
        </audio>
      </div>
    `;
    phoneticsSection.innerHTML += phoneticBlock;
  });
};

// Get the input word and search for its definition
const inputWord = document.getElementById("input") as HTMLInputElement;
const submitBtn = document.getElementById("submit") as HTMLButtonElement;

submitBtn.addEventListener("click", async (event: Event) => {
  event.preventDefault();
  const word: string = inputWord.value.trim();
  if (!word) return;

  try {
    const data = await searchWord(word);
    const meanings = extractWordDefinitions(data);
    displayWordDefinition(meanings);

    const phonetics = extractWordPhonetics(data);
    displayWordPhonetic(phonetics);
  } catch (error) {
    console.error("Error: ", error);
  }
});