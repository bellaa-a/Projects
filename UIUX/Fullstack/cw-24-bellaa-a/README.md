# Posts - A Simplified Social Media Platform

Posts is a minimalistic clone of <https://posts.cv/>, which is itself a simplified and scaled-down alternative to Twitter. We built this clone solely for educational purposes.

## Run locally

1. Clone this repository.

2. Navigate to the project directory.

3. To run the server API:

   1. Navigate to the `api` directory.
   2. Install the dependencies by running `pnpm install`.
   3. Create the database with `pnpm db:push`.
   4. Seed the database with `pnpm db:seed`.
   5. Run the server with `pnpm dev`. This will start the server on `http://localhost:3000`.

4. To run the application:

   1. Navigate to the `web` directory.
   2. Add a `.env` file to `web` directory with the following content:

        ```bash
        VITE_API_URL=http://localhost:3000
        ```

   3. Install the dependencies by running `pnpm install`.
   4. Run the application with `pnpm run dev`. This will start the application on `http://localhost:5173`.
