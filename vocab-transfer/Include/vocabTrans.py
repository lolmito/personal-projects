import time
import gspread
import pandas as pd
import requests
from oauth2client.service_account import ServiceAccountCredentials

# Step 1: Set up Google Sheets API authentication
scope = ["https://spreadsheets.google.com/feeds", "https://www.googleapis.com/auth/drive"]
creds = ServiceAccountCredentials.from_json_keyfile_name("vocabulary-transfer-50d459f1c356.json", scope)
print("Checking....")
client = gspread.authorize(creds)
print("DONE")
print("Getting Sheets.....")
# Step 2: Open the Google Sheet and specify the worksheet
sheet = client.open("Test Sheet").worksheet("Sheet1")
print("DONE")
# Step 3: Function to send words to a flashcard system (Example: Dummy API call)
def add_to_flashcards(word, definition):
    # Replace with the actual API endpoint of your flashcard system
    flashcard_api_url = "http://127.0.0.1:8765/"
    
    create_deck_payload = {
    "action": "createDeck",
    "version": 6,
    "params": {
        "deck": "Vocabulary"
    }
    }
    response = requests.post(flashcard_api_url, json=create_deck_payload)
    payload = {
        "action": "addNote",
        "version": 6,
        "params": {
            "note": {
                "deckName": "Vocabulary",  # This is where the words will go
                "modelName": "Basic",
                "fields": {
                    "Front": word,
                    "Back": definition
                },
                "tags": ["auto-added"]
            }
        }
    }
    response = requests.post(flashcard_api_url, json=payload)
    print(response.json())
    if response.status_code == 200:
        print(f"Successfully added: {word} - {definition}")
    else:
        print(f"Failed to add {word}. Error: {response.text}")

# Step 4: Sync function to check for new vocabulary
def sync_vocabulary():
    previous_words = set()

    while True:
        # Fetch data from Google Sheets
        data = sheet.get_all_records()
        df = pd.DataFrame(data)

        # Find new words that were not previously recorded
        new_words = set(df["Word"]) - previous_words

        for word in new_words:
            definition = df[df["Word"] == word]["Definition"].values[0]
            print(f"Adding: {word} - {definition}")
            add_to_flashcards(word, definition)  # Send to flashcard API

        # Update the previous words set
        previous_words = set(df["Word"])

        # Wait 10 minutes before checking again
        time.sleep(600)

# Step 5: Start the syncing process
sync_vocabulary()