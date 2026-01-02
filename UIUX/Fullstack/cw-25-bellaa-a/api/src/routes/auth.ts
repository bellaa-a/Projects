import { zValidator } from "@hono/zod-validator";
import { Hono } from "hono";
import { signInSchema, signUpSchema } from "../validators/schemas";
import { db } from "../db";
import { users } from "../db/schema";
import { eq } from "drizzle-orm";
import { HTTPException } from "hono/http-exception";

const authRoutes = new Hono();

authRoutes.post("/sign-in",
    zValidator("json", signInSchema), 
    async (c) => {
        const { username, password } = c.req.valid("json");

        const user = await db
            .select()
            .from(users)
            .where(eq(users.username, username))
            .get()

        if (!user) {
            throw new HTTPException(401, {
                message: "Incorrect username or password"
            })
        }

        if (user.password !== password) {
            throw new HTTPException(401, {
                message: "Incorrect username or password"
            })
        }

        return c.json({ 
            message: "You have been signed in!",
            user
        })
    }
);

authRoutes.post("/sign-up", 
    zValidator("json", signUpSchema), 
    async (c) => {
        const { name, username, password } = c.req.valid("json");

        const newUser = await db
            .insert(users)
            .values({
                name, 
                username, 
                password
            })
            .returning()
            .get()

        return c.json({ 
            message: "You have been signed up!",
            user: newUser
        }, 201)
    }
);

authRoutes.post("/sign-out", async (c) => {
    return c.json({ message: "You have been signed out!" })
});

export default authRoutes;