import os
import io
import sys
import dotenv
import time
from PIL import Image
from stability_sdk import client
import stability_sdk.interfaces.gooseai.generation.generation_pb2 as generation

dotenv.load_dotenv()

if "STABILITY_KEY" not in os.environ:
    sys.stderr.write("Error: STABILITY_KEY not found. Check your .env file.\n")
    sys.exit(1)

stability_api = client.StabilityInference(
    key=os.environ['STABILITY_KEY'],
    verbose=False,
    engine="stable-diffusion-xl-1024-v1-0"
)

if len(sys.argv) < 2:
    sys.stderr.write("Error: No prompt provided.\n")
    sys.exit(1)

prompt = sys.argv[1]

try:
    answers = stability_api.generate(
        prompt=prompt,
        steps=50,
        width=1024,
        height=1024,
        samples=1,
        sampler=generation.SAMPLER_K_DPMPP_2M
    )

    for resp in answers:
        for artifact in resp.artifacts:
            if artifact.type == generation.ARTIFACT_IMAGE:
                os.makedirs("images", exist_ok=True)

                timestamp = int(time.time())
                filename = f"images/{timestamp}.png"

                img = Image.open(io.BytesIO(artifact.binary))
                img.save(filename)

                os.system(f"nohup xdg-open {filename} >/dev/null 2>&1 &")

                print("done")
                sys.exit(0)

except Exception as e:
    sys.stderr.write(f"Error: {str(e)}\n")
    sys.exit(1)
