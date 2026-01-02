import { connection, db } from "./index";
import { posts } from "./schema";

async function seed() {
  console.log("Seeding database");

  console.log("Cleaning existing data...");
  await db.delete(posts);

  console.log("Resetting sequence...");
  await db.run(`DELETE FROM sqlite_sequence where name = 'posts'`);

  console.log("Inserting sample data...");
  await db
    .insert(posts)
    .values({
      content: "Post 1",
      date: new Date(),
    })
    .returning();

  await db
    .insert(posts)
    .values({
      content: "Post 2",
      date: new Date(),
    })
    .returning();

  await db
    .insert(posts)
    .values({
      content: "Post 3",
      date: new Date(),
    })
    .returning();
}

seed()
  .then(() => console.log("Seeding complete"))
  .catch((err) => console.error("Seeding failed", err))
  .finally(() => connection.close());
