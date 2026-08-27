# Graph RAG

**Graph RAG = RAG + Knowledge Graph**

The basic idea is:

> **Instead of only searching documents for similar text, Graph RAG understands the relationships between different pieces of information.**

For example, consider:

```text
Microsoft
   ↓ develops
Azure
   ↓ provides
AI Services
   ↓ used by
Company A
```

A normal RAG system mainly searches for relevant text chunks.

Graph RAG can understand the **relationships** between entities.

---

# Architecture

```text
                    Documents
                        ↓
                  Chunk Documents
                        ↓
               Extract Entities
                        ↓
          Extract Relationships
                        ↓
                Knowledge Graph
                        ↓
                  ┌───────────┐
User Query ──────→│  Retrieval│
                  └─────┬─────┘
                        ↓
             Find Relevant Nodes
              & Relationships
                        ↓
                  Context
                        ↓
                      LLM
                        ↓
                     Answer
```

## Simple Example

Suppose the question is:

> **"Which products are affected by Company A's supply problem?"**

The graph may contain:

```text
Company A
    │
    └── has problem ──→ Supply Issue
                            │
                            └── affects ──→ Product X
                            │
                            └── affects ──→ Product Y
```

Graph RAG can follow these relationships:

```text
Company A
    ↓
Supply Issue
    ↓
Affected Products
    ↓
Product X, Product Y
```

Then the LLM uses this information to generate the answer.

---

# Traditional RAG vs Graph RAG

### Traditional RAG

```text
Question
   ↓
Vector Search
   ↓
Relevant Text Chunks
   ↓
LLM
   ↓
Answer
```

It mainly asks:

> **"Which text is similar to my question?"**

### Graph RAG

```text
Question
   ↓
Graph Search
   ↓
Entities + Relationships
   ↓
Relevant Context
   ↓
LLM
   ↓
Answer
```

It can ask:

> **"What entities are involved, and how are they related?"**

---

# Key Idea

The main difference is:

```text
Traditional RAG
→ Finds relevant text

Graph RAG
→ Finds relevant entities + relationships
```

So, **Graph RAG is especially useful when relationships between information are important**, such as:

* People → Companies
* Customers → Products
* Products → Issues
* Employees → Projects
* Diseases → Symptoms → Treatments
* Companies → Acquisitions → Products

## In One Sentence

> **Graph RAG uses a knowledge graph to retrieve not only relevant information, but also the relationships between that information, and gives that context to the LLM to generate a better answer.**
