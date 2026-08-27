# Agentic RAG

**Agentic RAG = RAG + AI Agent**

The basic idea is:

> **Let the AI agent decide what information it needs, where to find it, and whether it has enough information to answer.**

Unlike traditional RAG, where the retrieval process is mostly fixed, **Agentic RAG can dynamically plan, search, evaluate, and search again if needed.**

---

# Architecture

```text
                    User Query
                        ↓
                    ┌───────┐
                    │ Agent │
                    └───┬───┘
                        ↓
                   Plan / Decide
                        ↓
             ┌──────────┼──────────┐
             ↓          ↓          ↓
         Vector DB    SQL DB     API/Web
             ↓          ↓          ↓
             └──────────┼──────────┘
                        ↓
                 Evaluate Results
                        ↓
                 ┌──────┴──────┐
                 ↓             ↓
             Not Enough      Enough
                 ↓             ↓
            Search Again    Generate
                 │             │
                 └──→ Agent    ↓
                            Answer
```

---

# Simple Example

Suppose the user asks:

> **"Why did Product X sales decrease?"**

The agent may work like this:

```text
User Question
     ↓
Agent
     ↓
"I need sales data."
     ↓
Search Sales Database
     ↓
"Sales decreased by 20%."
     ↓
"I need to know why."
     ↓
Search Company Documents
     ↓
"Supply problems were reported."
     ↓
"I have enough information."
     ↓
Generate Answer
```

The important point is that **the agent decides what to do next based on what it finds.**

---

# Traditional RAG vs Agentic RAG

### Traditional RAG

```text
Question
   ↓
Retrieve
   ↓
LLM
   ↓
Answer
```

### Agentic RAG

```text
Question
   ↓
Agent
   ↓
Plan
   ↓
Retrieve
   ↓
Evaluate
   ↓
Need more information?
   ├── Yes → Search again
   └── No  → Answer
```

---

# Key Idea

The main difference is:

```text
Traditional RAG
→ Retrieve information and generate an answer.

Agentic RAG
→ Decide what to retrieve, retrieve it, evaluate it,
  retrieve more if necessary, and then generate an answer.
```

## In One Sentence

> **Agentic RAG is a RAG system where an AI agent dynamically controls the retrieval process, uses different tools or data sources when needed, and iterates until it has enough information to answer.**
