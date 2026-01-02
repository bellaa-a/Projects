import { zValidator } from "@hono/zod-validator";
import { Hono } from "hono";
import { signInSchema, signUpSchema } from "../validators/schemas";
import { users } from "../db/schema";
import { db } from "../db";
import { eq } from "drizzle-orm";
import { HTTPException } from "hono/http-exception";
import { hash, verify } from "@node-rs/argon2";
import { lucia } from "../db/auth";

const authRoutes = new Hono();

// encrypt / decrypt
// encode / decode
// hashing

// Recommended minimum parameters for Argon2 hashing
const hashOptions = {
  memoryCost: 19456,
  timeCost: 2,
  outputLen: 32,
  parallelism: 1,
}

authRoutes.post("/sign-in", 
  zValidator("json", signInSchema),
  async (c) => {
    const { username, password } = c.req.valid("json");
    
    const user = await db
      .select()
      .from(users)
      .where(eq(users.username, username))
      .get();

    if (!user) {
      throw new HTTPException(401, {
        message: "Invalid username or password",
      });
    }

    const validPassword = verify(user.password,  password, hashOptions);
    if (!validPassword) {
      throw new HTTPException(401, {
        message: "Invalid username or password",
      });
    }

    const { password: _, ...rest } = user;

    // crete a session for the uer
    const session = await lucia.createSession(user.id, {});

    return c.json({ 
      message: "You have been signed in!",
      session,
      user: rest
     });
  }
);

authRoutes.post("/sign-up",
  zValidator("json", signUpSchema), 
  async (c) => {
    const { name, username, password } = c.req.valid("json");
    
    const hashedPassword = await hash(password, hashOptions);

    const newUser = await db
      .insert(users)
      .values(
        {
          name,
          username,
          password: hashedPassword
        }
      )
      .returning()
      .get();

    return c.json({ 
      message: "You have been signed up!",
      user: {
        id: newUser.id,
        name: newUser.name,
        username: newUser.username,
      }
    }, 201);
  }
);

authRoutes.post("/sign-out",
  async (c) => {

    // invalidate (expire or delete) the user session

    return c.json({ message: "You have been signed out!" });
  }
);

export default authRoutes;