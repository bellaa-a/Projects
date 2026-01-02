import { DrizzleSQLiteAdapter } from "@lucia-auth/adapter-drizzle";
import { Lucia } from "lucia";
import { db } from ".";
import { sessions, users } from "./schema";
import { TimeSpan } from "lucia";

const adapter = new DrizzleSQLiteAdapter(db, sessions, users);

export const lucia = new Lucia(adapter, {
    sessionCookie: {
      attributes: {
        secure: process.env.NODE_ENV === "production",
        sameSite: "none",
      },
    },
    // Look here 👇
    sessionExpiresIn: new TimeSpan(1, "m"), // 1 minutes
  });

declare module "lucia" {
    interface Register {
        Lucia: typeof lucia;
        UserId: number;
    }
}